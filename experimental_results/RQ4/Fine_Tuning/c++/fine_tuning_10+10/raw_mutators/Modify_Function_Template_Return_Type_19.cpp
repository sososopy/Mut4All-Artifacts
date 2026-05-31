//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_Return_Type_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Function_Template_Return_Type_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        //Filter nodes in header files
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                FT->getLocation()))
            return;

        for (auto *Spec : FT->specializations()) {
            if (Spec->getReturnType()->isUndeducedAutoType()) {
                //Get the source code text of target node
                auto funcDecl = Spec->getTemplatedDecl();
                std::string newReturnType = funcDecl->getReturnType().getAsString();
                newReturnType += "&"; // Change return type to reference

                //Perform mutation on the source code text by applying string replacement
                TheRewriter.ReplaceText(funcDecl->getReturnTypeSourceRange(), newReturnType);

                // If necessary, implement a minimal body
                if (!funcDecl->hasBody()) {
                    std::string funcBody = "{ static " + newReturnType + " value{}; return value; }";
                    TheRewriter.InsertTextAfterToken(funcDecl->getEndLoc(), funcBody);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(functionDecl(returns(autoType())))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}