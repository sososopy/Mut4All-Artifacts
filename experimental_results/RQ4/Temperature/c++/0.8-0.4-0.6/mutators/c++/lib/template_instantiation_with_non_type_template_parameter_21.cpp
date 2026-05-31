//source file
#include "../include/template_instantiation_with_non_type_template_parameter_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (!FD->hasBody())
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Rewrite.getLangOpts();

        // Get the source code text of target node
        std::string functionText = Lexer::getSourceText(
            CharSourceRange::getTokenRange(FD->getSourceRange()), SM, LangOpts).str();

        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<int N> ";
        std::string paramName = FD->parameters()[0]->getNameAsString();
        std::string replacement = paramName + " + N";

        size_t pos = functionText.find(paramName);
        if (pos != std::string::npos) {
            functionText.replace(pos, paramName.length(), replacement);
        }

        functionText.insert(0, templateParam);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isExpansionInMainFile(), unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}