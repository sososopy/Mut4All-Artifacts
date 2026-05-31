//source file
#include "../include/introduce_pseudo_destructor_call_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodDecl")) {
        if (!methodDecl || !Result.Context->getSourceManager().isWrittenInMainFile(methodDecl->getLocation()))
            return;

        if (auto *memberExpr = Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {
            // Ensure we are dealing with a pointer or reference
            if (memberExpr->isArrow()) {
                //Get the source code text of target node
                std::string memberText = Lexer::getSourceText(
                    CharSourceRange::getTokenRange(memberExpr->getSourceRange()),
                    *Result.SourceManager, Result.Context->getLangOpts()).str();

                //Perform mutation on the source code text by applying string replacement
                std::string mutatedText = "(*" + memberText + ").~NonExistentType()";
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(memberExpr->getSourceRange(), mutatedText);
            }
        }
    }
}

void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation
    auto memberExprMatcher = memberExpr(hasType(pointsTo(qualType()))).bind("memberExpr");
    auto methodDeclMatcher = cxxMethodDecl(isDefinition()).bind("methodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(memberExprMatcher, &callback);
    matchFinder.addMatcher(methodDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}