//source file
#include "../include/implicit_return_type_and_function_pointer_604.h"

// ========================================================================================================
#define MUT604_OUTPUT 1

void MutatorFrontendAction_604::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (FD->hasBody() && !FD->isMain()) {
            // Remove explicit return type
            SourceRange returnTypeRange = FD->getReturnTypeSourceRange();
            Rewrite.RemoveText(returnTypeRange);

            // Find a variable to misuse as a function pointer
            for (const auto *stmt : FD->getBody()->children()) {
                if (const auto *declStmt = dyn_cast<DeclStmt>(stmt)) {
                    for (const auto *decl : declStmt->decls()) {
                        if (const auto *varDecl = dyn_cast<VarDecl>(decl)) {
                            if (!varDecl->getType()->isFunctionPointerType()) {
                                TargetVariable = varDecl;
                                break;
                            }
                        }
                    }
                }
                if (TargetVariable) break;
            }

            if (TargetVariable) {
                // Insert misuse of variable as function pointer
                std::string misuseText = "\n/*mut604*/" + TargetVariable->getNameAsString() + "(10, 20);\n";
                Rewrite.InsertTextAfterToken(FD->getBodyRBrace(), misuseText);
            }
        }
    }
}

void MutatorFrontendAction_604::MutatorASTConsumer_604::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}