//source file
#include "../include/Incorrect_Scope_Deduction_Guide_207.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_207::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        //Filter nodes in header files
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        if (visitedNamespaces.count(NS))
            return;

        visitedNamespaces.insert(NS);

        for (const auto *D : NS->decls()) {
            if (const auto *TD = llvm::dyn_cast<clang::ClassTemplateDecl>(D)) {
                //Get the source code text of target node
                std::string templateName = TD->getNameAsString();
                std::string guide = "\n/*mut207*/template<class U> " + templateName + "(U) -> " + templateName + "<U>;\n";
                //Perform mutation on the source code text by applying string replacement
                //Replace the original AST node with the mutated one
                Rewrite.InsertTextAfterToken(NS->getEndLoc(), guide);
            }
        }
    }
}
  
void MutatorFrontendAction_207::MutatorASTConsumer_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl(hasDescendant(classTemplateDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}