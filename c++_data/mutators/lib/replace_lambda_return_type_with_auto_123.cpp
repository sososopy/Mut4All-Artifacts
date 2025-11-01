//source file
#include "../include/Replace_Lambda_Return_Type_With_Auto_123.h"

// ========================================================================================================
#define MUT123_OUTPUT 1

void MutatorFrontendAction_123::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        //Filter nodes in header files
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *LambdaTypeInfo = LambdaExpr->getLambdaClass()->getLambdaTypeInfo();
        if (LambdaTypeInfo && LambdaTypeInfo->getType()->isFunctionProtoType()) {
            //Get the source code text of target node
            auto lambdaRange = LambdaExpr->getSourceRange();
            std::string lambdaText = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(lambdaRange),
                                                          *Result.SourceManager, Result.Context->getLangOpts()));

            //Perform mutation on the source code text by applying string replacement
            size_t pos = lambdaText.find("->");
            if (pos != std::string::npos) {
                pos = lambdaText.find(" ", pos);
                if (pos != std::string::npos) {
                    lambdaText.replace(pos + 1, 4, "auto");
                }
            }

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
        }
    }
}

void MutatorFrontendAction_123::MutatorASTConsumer_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasType(cxxRecordDecl())).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}