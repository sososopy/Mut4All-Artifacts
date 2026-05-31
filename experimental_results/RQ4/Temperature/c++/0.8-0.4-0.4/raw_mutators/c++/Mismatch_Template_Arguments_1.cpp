//header file
#pragma once
#include "Mutator_base.h"

/**
 * mismatch_template_arguments_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::FunctionTemplateDecl *> functionTemplates;
    };
};

//source file
#include "../include/mismatch_template_arguments_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        //Filter nodes in header files
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FT->getLocation()))
            return;

        functionTemplates.push_back(FT);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        for (const auto *FT : functionTemplates) {
            if (FD->getPrimaryTemplate() == FT) {
                //Get the source code text of target node
                std::string callExpr = FD->getNameInfo().getAsString() + "<int, int>(";
                for (unsigned i = 0; i < FD->getNumParams(); ++i) {
                    if (i > 0) callExpr += ", ";
                    callExpr += "0"; // Use 0 for simplicity
                }
                callExpr += ");\n";

                //Perform mutation on the source code text by applying string replacement
                Rewrite.InsertTextAfter(FD->getBody()->getBeginLoc(), "/*mut1*/" + callExpr);
            }
        }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("FunctionTemplate");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}