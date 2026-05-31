//source file
#include "../include/incomplete_template_class_specialization_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (!TD->isThisDeclarationADefinition()) {
        //Get the source code text of target node
        auto templateDeclStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                       TD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string memberFunction = "public:\n    void memberFunction() {\n        someMember = 0;\n    }\n";
        if (templateDeclStr.rfind(';') != std::string::npos) {
            templateDeclStr.insert(templateDeclStr.rfind(';'), memberFunction);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDeclStr);
      }
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}