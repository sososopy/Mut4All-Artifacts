//source file
#include "../include/Incorrect_Assignment_To_Struct_300.h"

// ========================================================================================================
#define MUT300_OUTPUT 1

void MutatorFrontendAction_300::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("structVar")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        // Record the struct variable declaration
        structVarDecl = VD;
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (structVarDecl && FD->hasBody()) {
            // Get the source code text of target node
            auto funcBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
            std::string structName = structVarDecl->getType().getAsString();
            std::string varName = structVarDecl->getNameAsString();
            std::string mutation = "\nint mutInt = 42;\n" + varName + " = mutInt; // Incorrect assignment\n";

            // Perform mutation on the source code text by applying string replacement
            funcBody.insert(funcBody.find("{") + 1, "/*mut300*/" + mutation);

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), funcBody);
        }
    }
}
  
void MutatorFrontendAction_300::MutatorASTConsumer_300::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher structVarMatcher = varDecl(hasType(recordDecl(isStruct()))).bind("structVar");
    DeclarationMatcher funcDeclMatcher = functionDecl(hasBody(stmt())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structVarMatcher, &callback);
    matchFinder.addMatcher(funcDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}