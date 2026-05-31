//source file
#include "../include/Mutator_Modify_Dependent_Template_Specialization_Instantiation_Order_420.h"

// ========================================================================================================
#define MUT420_OUTPUT 1

void MutatorFrontendAction_420::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplateDecl")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templates.push_back(CTD);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpecializationDecl")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      specializations.push_back(CTSD);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      uses.push_back(VD);
    }
    else if (auto *TSD = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecializationType")) {
      //Filter nodes in header files
      if (!TSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Result.SourceManager->getSpellingLoc(TSD->getTemplateName().getAsTemplateDecl()->getLocation())))
        return;
      //Perform mutation logic
      if (templates.empty() || specializations.empty() || uses.empty()) return;
      
      // Choose a random template and specialization
      size_t templateIdx = getrandom::getRandomIndex(templates.size() - 1);
      size_t specIdx = getrandom::getRandomIndex(specializations.size() - 1);
      size_t useIdx = getrandom::getRandomIndex(uses.size() - 1);
      
      const clang::ClassTemplateDecl* CTD = templates[templateIdx];
      const clang::ClassTemplateSpecializationDecl* CTSD = specializations[specIdx];
      const clang::VarDecl* VD = uses[useIdx];
      
      // Check if the specialization belongs to the chosen template
      if (CTSD->getSpecializedTemplate() != CTD) return;
      
      // Get source ranges
      SourceRange templateRange = CTD->getSourceRange();
      SourceRange specRange = CTSD->getSourceRange();
      SourceRange useRange = VD->getSourceRange();
      
      SourceManager &SM = Rewrite.getSourceMgr();
      
      // Determine order: if specialization is before use, move it after
      if (SM.isBeforeInTranslationUnit(specRange.getBegin(), useRange.getBegin())) {
        // Move specialization after use
        std::string specText = stringutils::rangetoStr(SM, specRange);
        Rewrite.RemoveText(specRange);
        Rewrite.InsertTextAfterToken(useRange.getEnd(), "\n" + specText);
      } else {
        // Specialization is after use or no specialization exists
        // Introduce a new dependent specialization
        // Find another template to create dependent type
        if (templates.size() < 2) return;
        size_t otherTemplateIdx = getrandom::getRandomIndex(templates.size() - 1);
        if (otherTemplateIdx == templateIdx) otherTemplateIdx = (otherTemplateIdx + 1) % templates.size();
        const clang::ClassTemplateDecl* otherCTD = templates[otherTemplateIdx];
        
        // Create new specialization text
        std::string newSpec = "template<typename T> class " + CTD->getNameAsString() + "<" + otherCTD->getNameAsString() + "<T>*> {};\n";
        
        // Insert before the use
        Rewrite.InsertTextBefore(useRange.getBegin(), "/*mut420*/" + newSpec);
      }
    }
}
  
void MutatorFrontendAction_420::MutatorASTConsumer_420::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("ClassTemplateDecl");
    DeclarationMatcher specMatcher = classTemplateSpecializationDecl().bind("ClassTemplateSpecializationDecl");
    DeclarationMatcher varMatcher = varDecl(hasType(templateSpecializationType())).bind("VarDecl");
    TypeMatcher typeMatcher = templateSpecializationType().bind("TemplateSpecializationType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(specMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}