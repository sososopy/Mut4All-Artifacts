//source file
#include "../include/Mutator_Introduce_Ill_Formed_Partial_Specialization_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("BaseTemplate")) {
      //Filter nodes in header files
      if (!BTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BTD->getLocation()))
        return;
      //Record candidate base templates
      candidateBaseTemplates.push_back(BTD);
    }
    else if (auto *PTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("PrimaryTemplate")) {
      //Filter nodes in header files
      if (!PTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PTD->getLocation()))
        return;
      if (!PTD->isThisDeclarationADefinition())
        return;
      //Get the source code text of target node
      auto *Templ = PTD->getTemplatedDecl();
      if (!Templ)
        return;
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), PTD->getSourceRange());
      //Find a base template
      const clang::ClassTemplateDecl* baseTemplate = nullptr;
      if (!candidateBaseTemplates.empty()) {
        size_t idx = getrandom::getRandomIndex(candidateBaseTemplates.size() - 1);
        baseTemplate = candidateBaseTemplates[idx];
      }
      //Perform mutation on the source code text by applying string replacement
      std::string baseName;
      if (baseTemplate) {
        baseName = baseTemplate->getNameAsString();
      } else {
        baseName = "Base";
        //Insert a simple base template declaration before the specialization
        std::string baseDecl = "template<typename> struct " + baseName + ";";
        Rewrite.InsertTextBefore(PTD->getBeginLoc(), baseDecl + "\n");
      }
      std::string templName = PTD->getNameAsString();
      std::string mutatedText = "template<typename U> struct " + templName + "<int> : " + baseName + "<" + templName + "<U>> {};";
      mutatedText = "/*mut191*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PTD->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher baseMatcher = classTemplateDecl().bind("BaseTemplate");
    DeclarationMatcher primaryMatcher = classTemplateDecl().bind("PrimaryTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(primaryMatcher, &callback);
    matchFinder.matchAST(Context);
}