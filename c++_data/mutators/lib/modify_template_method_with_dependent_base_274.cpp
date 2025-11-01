//source file
#include "../include/modify_template_method_with_dependent_base_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TemplateMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string baseClass = "class Base { public: bool IsValid() const { return true; } };";
        std::string methodPointer = "return (this->*static_cast<bool (Base::*)()const>(&Base::IsValid))();";
        declaration.insert(declaration.find("{") + 1, "\n/*mut274*/" + methodPointer + "\n");
        declaration = baseClass + "\n" + declaration;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("TemplateMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}