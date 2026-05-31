//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Default_Arg_To_Template_Friend_Function_219
 */ 

class MutatorFrontendAction_219 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(219)
private:
    class MutatorASTConsumer_219 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_219(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *targetFunction = nullptr;
    };
};

//source file
#include "../include/Add_Default_Arg_To_Template_Friend_Function_219.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("friendDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        auto *funcDecl = FD->getFriendTheardDecl();
        if (!funcDecl || !funcDecl->isFunction())
            return;

        auto *func = cast<clang::FunctionDecl>(funcDecl);
        bool hasNonDefaultParam = false;
        for (const auto *param : func->parameters()) {
            if (!param->hasDefaultArg()) {
                hasNonDefaultParam = true;
                break;
            }
        }
        if (!hasNonDefaultParam)
            return;

        auto *lastParam = func->getParamDecl(func->getNumParams() - 1);
        if (!lastParam->hasDefaultArg()) {
            SourceLocation endLoc = lastParam->getNameInfo().getLoc();
            Rewrite.InsertText(endLoc, " = 0", true, true);
            targetFunction = func;
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE)
            return;

        auto *callee = CE->getDirectCallee();
        if (!callee || callee != targetFunction)
            return;

        std::string callText = stringutils::rangetoStr(SM, CE->getSourceRange());
        size_t lastCommaPos = callText.rfind(',');
        if (lastCommaPos != std::string::npos) {
            callText.erase(lastCommaPos);
            callText.erase(std::remove(callText.begin(), callText.end(), ' '), callText.end());
            Rewrite.ReplaceText(CE->getSourceRange(), callText);
        } else {
            callText = callText.substr(0, callText.find('(')+1) + callText.substr(callText.find(')'));
            Rewrite.ReplaceText(CE->getSourceRange(), callText);
        }
    }
}
  
void MutatorFrontendAction_219::MutatorASTConsumer_219::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher friendMatcher = friendDecl(
        hasParent(cxxRecordDecl(isTemplateInstantiated())),
        hasFriend(functionDecl().bind("funcDecl"))
    ).bind("friendDecl");
    DeclarationMatcher callMatcher = callExpr().bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(friendMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}