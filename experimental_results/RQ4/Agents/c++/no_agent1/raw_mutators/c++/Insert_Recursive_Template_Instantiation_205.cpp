//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> visitedFunctions;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (visitedFunctions.find(FD) != visitedFunctions.end())
            return;

        visitedFunctions.insert(FD);

        if (FD->isTemplateInstantiation()) {
            auto functionName = FD->getNameAsString();
            auto templateArgs = FD->getTemplateSpecializationArgs();
            if (!templateArgs)
                return;

            std::string newTemplateInstantiation = functionName + "<";
            for (unsigned i = 0; i < templateArgs->size(); ++i) {
                if (i > 0) newTemplateInstantiation += ", ";
                newTemplateInstantiation += templateArgs->get(i).getAsType().getAsString();
            }
            newTemplateInstantiation += ">();";

            SourceLocation insertLocation = FD->getEndLoc().getLocWithOffset(1);
            Rewrite.InsertText(insertLocation, "\n/*mut205*/" + newTemplateInstantiation);
        }
    }
}

void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}