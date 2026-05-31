//source file
#include "../include/modify_template_function_return_type_639.h"

// ========================================================================================================
#define MUT639_OUTPUT 1

void MutatorFrontendAction_639::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RT = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
        if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                RT->getLocation()))
            return;

        if (const auto *Body = RT->getBody()) {
            for (const auto *Stmt : Body->children()) {
                if (const auto *ReturnStmt = dyn_cast<clang::ReturnStmt>(Stmt)) {
                    const auto *RetExpr = ReturnStmt->getRetValue();
                    if (RetExpr && isa<clang::CXXConstructExpr>(RetExpr)) {
                        auto ReturnRange = ReturnStmt->getSourceRange();
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ReturnRange), "return {};");
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_639::MutatorASTConsumer_639::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), returns(hasDescendant(templateTypeParmType()))).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}