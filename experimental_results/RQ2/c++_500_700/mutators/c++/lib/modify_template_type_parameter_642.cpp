//source file
#include "../include/modify_template_type_parameter_642.h"

// ========================================================================================================
#define MUT642_OUTPUT 1

void MutatorFrontendAction_642::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LOM = Result.Context->getLangOpts();

        // Get the source code text of target node
        std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), SM, LOM).str();

        // Perform mutation on the source code text by applying string replacement
        std::string newText = originalText;
        size_t pos = newText.find(">");
        if (pos != std::string::npos) {
            std::string insertion = ", typename M = int ExampleStruct::*";
            newText.insert(pos, insertion);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), newText);
    }
}
  
void MutatorFrontendAction_642::MutatorASTConsumer_642::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(templateTypeParmDecl())).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}