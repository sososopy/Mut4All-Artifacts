//header file
#pragma once
#include "Mutator_base.h"

/**
 * complete_incomplete_class_definitions_33
 */ 
class MutatorFrontendAction_33 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(33)

private:
    class MutatorASTConsumer_33 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_33(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::NamedDecl*> processedDecls;
    };
};

//source file
#include "../include/complete_incomplete_class_definitions_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("IncompleteTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        if (processedDecls.find(CTD) != processedDecls.end())
            return;

        processedDecls.insert(CTD);

        if (auto *RDecl = CTD->getTemplatedDecl()) {
            if (!RDecl->isCompleteDefinition()) {
                std::string className = RDecl->getNameAsString();
                std::string definition = "template<typename T> struct " + className + " { int dummyVar; " + className + "() : dummyVar(0) {} };";
                std::string comment = "/*mut33*/";
                Rewrite.ReplaceText(RDecl->getSourceRange(), comment + definition);
            }
        }
    }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(unless(isDefinition())).bind("IncompleteTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}