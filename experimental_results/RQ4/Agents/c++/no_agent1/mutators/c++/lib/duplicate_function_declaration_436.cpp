//source file
#include "../include/duplicate_function_declaration_436.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->isThisDeclarationADefinition() && !FD->isMain()) {
            std::string funcName = FD->getNameAsString();
            if (visitedFunctions.find(funcName) == visitedFunctions.end()) {
                visitedFunctions.insert(funcName);
                
                // Get the source code text of target node
                auto funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
                
                // Perform mutation by duplicating the function declaration
                std::string duplicateDecl = "\n/*mut436*/" + funcDecl + ";";
                
                // Replace the original AST node with the mutated one
                Rewrite.InsertTextAfterToken(FD->getEndLoc(), duplicateDecl);
            }
        }
    }
}

void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}