//header file
#pragma once
#include "Mutator_base.h"

/**
 * Split_Template_Deduction_Guide_Across_Modules_445
 */ 
class MutatorFrontendAction_445 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(445)

private:
    class MutatorASTConsumer_445 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_445(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Split_Template_Deduction_Guide_Across_Modules_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        const NamespaceDecl *ns = nullptr;
        const DeclContext *ctx = MT->getDeclContext();
        while (ctx && !ns) {
            if (const NamespaceDecl *nd = dyn_cast<NamespaceDecl>(ctx)) {
                ns = nd;
                break;
            }
            ctx = ctx->getParent();
        }

        std::string namespaceStr;
        if (ns) {
            namespaceStr = ns->getNameAsString();
        }

        std::string recordName = MT->getNameAsString();

        std::string codeA = "module A;\nimport H;\n";
        std::string codeB = "module B;\nimport H;\n";

        if (!namespaceStr.empty()) {
            codeA += namespaceStr + "::";
            codeB += namespaceStr + "::";
        }

        codeA += recordName + " x1 = " + codeA.substr(0, codeA.length() - 1) + "(42);\n";
        codeB += recordName + " x2 = " + codeB.substr(0, codeB.length() - 1) + "(3.14);\n";

        SourceManager &SM = *Result.SourceManager;
        FileID FID = SM.getFileID(MT->getLocation());
        SourceLocation EndLoc = SM.getLocForEndOfFile(FID);

        Rewrite.InsertText(EndLoc, codeA + codeB, true, true);
    }
}
  
void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxDeductionGuideDecl())).bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}