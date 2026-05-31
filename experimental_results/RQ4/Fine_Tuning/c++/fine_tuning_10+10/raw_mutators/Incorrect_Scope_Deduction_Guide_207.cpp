//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Scope_Deduction_Guide_207
 */ 
class MutatorFrontendAction_207 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(207)

private:
    class MutatorASTConsumer_207 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_207(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::unordered_set<const clang::NamespaceDecl *> visitedNamespaces;
    };
};

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