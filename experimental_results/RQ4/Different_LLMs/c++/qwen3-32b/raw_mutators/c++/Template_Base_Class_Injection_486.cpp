//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Base_Class_Injection_486
 */ 
class MutatorFrontendAction_486 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(486)

private:
    class MutatorASTConsumer_486 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_486(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Template_Base_Class_Injection_486.h"

// ========================================================================================================
#define MUT486_OUTPUT 1

void MutatorFrontendAction_486::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if (DL->getNumBases() == 0)
            return;
        Rewrite.InsertText(DL->getBeginLoc(), "template <typename T>\n", true, true);
        for (const auto &base : DL->bases()) {
            clang::AccessSpecifier access = base.getAccessSpecifier();
            std::string accessStr;
            switch (access) {
                case clang::AS_public: accessStr = "public"; break;
                case clang::AS_private: accessStr = "private"; break;
                case clang::AS_protected: accessStr = "protected"; break;
                default: accessStr = "";
            }
            std::string replacement = ": " + accessStr + " T";
            Rewrite.ReplaceText(base.getSourceRange(), replacement);
        }
    }
}

void MutatorFrontendAction_486::MutatorASTConsumer_486::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(anything())).bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}