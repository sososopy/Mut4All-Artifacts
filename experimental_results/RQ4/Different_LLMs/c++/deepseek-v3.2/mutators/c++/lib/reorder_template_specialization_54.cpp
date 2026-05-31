//source file
#include "../include/Mutator_Reorder_T1emplate_Specialization_54.h"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      if (!CTSD->isExplicitSpecialization())
        return;
      //Record the node information to be used in the mutation process
      specializations.push_back(CTSD);
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("PrimaryTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      // Check if there are any specializations by iterating through recorded specializations
      bool hasSpecs = false;
      for (auto* spec : specializations) {
        if (spec->getSpecializedTemplate() == CTD) {
          hasSpecs = true;
          break;
        }
      }
      if (!hasSpecs)
        return;
      //Record the node information to be used in the mutation process
      primaryTemplates.push_back(CTD);
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("AllTemplates")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
        
      if (!primaryTemplates.empty() && !specializations.empty()) {
        // Choose a random primary template and its specialization
        size_t primaryIdx = getrandom::getRandomIndex(primaryTemplates.size() - 1);
        const clang::ClassTemplateDecl* primary = primaryTemplates[primaryIdx];
        
        // Find specializations of this primary template
        std::vector<const clang::ClassTemplateSpecializationDecl*> matchingSpecs;
        for (auto* spec : specializations) {
          if (spec->getSpecializedTemplate() == primary) {
            matchingSpecs.push_back(spec);
          }
        }
        
        if (!matchingSpecs.empty()) {
          size_t specIdx = getrandom::getRandomIndex(matchingSpecs.size() - 1);
          const clang::ClassTemplateSpecializationDecl* spec = matchingSpecs[specIdx];
          
          // Get source ranges
          SourceRange primaryRange = primary->getSourceRange();
          SourceRange specRange = spec->getSourceRange();
          
          if (primaryRange.isValid() && specRange.isValid()) {
            SourceManager &SM = Rewrite.getSourceMgr();
            
            // Get the source code
            std::string primaryCode = stringutils::rangetoStr(SM, primaryRange);
            std::string specCode = stringutils::rangetoStr(SM, specRange);
            
            // Check if primary appears before specialization
            if (SM.isBeforeInTranslationUnit(primaryRange.getBegin(), specRange.getBegin())) {
              // Create forward declaration for primary template
              std::string forwardDecl;
              if (auto* templParams = primary->getTemplateParameters()) {
                forwardDecl = "template<";
                bool first = true;
                for (unsigned i = 0; i < templParams->size(); ++i) {
                  if (!first) forwardDecl += ", ";
                  first = false;
                  if (auto* TTP = dyn_cast<TemplateTypeParmDecl>(templParams->getParam(i))) {
                    forwardDecl += "typename ";
                    if (TTP->getIdentifier()) {
                      forwardDecl += TTP->getIdentifier()->getName().str();
                    } else {
                      forwardDecl += "T" + std::to_string(i);
                    }
                  }
                }
                forwardDecl += "> class " + primary->getNameAsString() + ";\n";
              }
              
              // Perform mutation: move specialization before primary
              // First remove both declarations
              Rewrite.RemoveText(primaryRange);
              Rewrite.RemoveText(specRange);
              
              // Insert forward declaration, then specialization, then primary
              Rewrite.InsertTextBefore(primaryRange.getBegin(), 
                "/*mut54*/" + forwardDecl + specCode + "\n" + primaryCode);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher specMatcher = classTemplateSpecializationDecl().bind("Specialization");
    DeclarationMatcher primaryMatcher = classTemplateDecl().bind("PrimaryTemplate");
    DeclarationMatcher allTemplatesMatcher = classTemplateDecl().bind("AllTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(specMatcher, &callback);
    matchFinder.addMatcher(primaryMatcher, &callback);
    matchFinder.addMatcher(allTemplatesMatcher, &callback);
    matchFinder.matchAST(Context);
}