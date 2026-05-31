//source file
#include "../include/add_invalid_template_argument_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplateCall")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        std::string funcCall = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(FT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();

        size_t angleBracketPos = funcCall.find('<');
        if (angleBracketPos != std::string::npos) {
            funcCall.insert(angleBracketPos + 1, "int, 42, ");
        }

        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(FT->getSourceRange()), funcCall);
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::functionTemplateDecl().bind("FunctionTemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}