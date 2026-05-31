//header file
#pragma once
#include "Mutator_base.h"

/**
 * Move_Deduction_Guide_Scope_207
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
    };
};

//source file
#include "../include/Move_Deduction_Guide_Scope_207.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_207::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("deductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;

        std::string dgName = DG->getNameAsString();
        if (dgName.empty())
            return;

        clang::DeclContext *DC = DG->getDeclContext();
        clang::NamespaceDecl *NS = nullptr;
        if (auto *NSD = dyn_cast<clang::NamespaceDecl>(DC)) {
            NS = NSD;
        } else if (DC->isTranslationUnit()) {
            NS = nullptr;
        } else {
            return;
        }

        clang::NamedDecl *foundDecl = nullptr;
        for (auto *D : NS ? NS->decls() : Result.Context->getTranslationUnitDecl()->decls()) {
            if (auto *RD = dyn_cast<clang::CXXRecordDecl>(D)) {
                if (RD->getNameAsString() == dgName && RD->isTemplated()) {
                    foundDecl = RD;
                    break;
                }
            }
        }

        if (!foundDecl)
            return;

        SourceManager &SM = *Result.SourceManager;
        SourceLocation dgStart = DG->getSourceRange().getBegin();
        SourceLocation dgEnd = DG->getSourceRange().getEnd();
        std::string dgCode = stringutils::rangetoStr(SM, DG->getSourceRange());

        if (NS) {
            SourceLocation nsStart = NS->getSourceRange().getBegin();
            Rewrite.InsertText(nsStart, dgCode, true, true);
            Rewrite.RemoveText(dgStart, dgEnd);
        } else {
            std::string newNs = "MutatedNS";
            std::string newNsCode = "namespace " + newNs + " {\n" + dgCode + "\n}\n";
            SourceLocation endOfFile = SM.getLocForEndOfFile(SM.getMainFileID());
            Rewrite.InsertText(endOfFile, newNsCode, true, true);
            Rewrite.RemoveText(dgStart, dgEnd);
        }
    }
}
  
void MutatorFrontendAction_207::MutatorASTConsumer_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDeductionGuide()).bind("deductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}