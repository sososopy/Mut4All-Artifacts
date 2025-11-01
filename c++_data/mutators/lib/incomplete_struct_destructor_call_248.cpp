//source file
#include "../include/Incomplete_Struct_Destructor_Call_248.h"

// ========================================================================================================
#define MUT248_OUTPUT 1

void MutatorFrontendAction_248::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Get the source code text of target node
        auto functionBody = FD->getBody();
        if (!functionBody)
            return;

        for (auto *stmt : functionBody->children()) {
            if (auto *expr = llvm::dyn_cast<clang::CXXMemberCallExpr>(stmt)) {
                if (auto *methodDecl = expr->getMethodDecl()) {
                    if (llvm::isa<clang::CXXDestructorDecl>(methodDecl)) {
                        auto *recordDecl = methodDecl->getParent();
                        if (recordDecl && !recordDecl->isCompleteDefinition()) {
                            // Perform mutation on the source code text by applying string replacement
                            auto destructorCall = stringutils::rangetoStr(*(Result.SourceManager), expr->getSourceRange());
                            destructorCall = "/*mut248*/" + destructorCall;
                            // Replace the original AST node with the mutated one
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(expr->getSourceRange()), destructorCall);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_248::MutatorASTConsumer_248::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(cxxMemberCallExpr(callee(cxxDestructorDecl())))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}