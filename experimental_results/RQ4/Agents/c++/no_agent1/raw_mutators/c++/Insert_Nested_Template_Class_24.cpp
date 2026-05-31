//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Nested_Template_Class_24
 */ 

class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)
private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> processedClasses;
    };
};

//source file
#include "../include/insert_nested_template_class_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(CRD->getLocation()))
            return;
        if (!CRD->isCompleteDefinition() || CRD->isLambda() || processedClasses.count(CRD))
            return;

        processedClasses.insert(CRD);

        std::string templateCode = R"(
        template<typename T>
        class NestedTemplate {
        public:
            T value;
            NestedTemplate(T val) : value(val) {}
            T getValue() const { return value; }
        };
        )";

        std::string insertion = "\n/*mut24*/" + templateCode;
        Rewrite.InsertTextAfterToken(CRD->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}