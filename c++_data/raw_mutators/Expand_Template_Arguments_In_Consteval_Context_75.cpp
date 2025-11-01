//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Template_Arguments_In_Consteval_Context_75
 */ 
class MutatorFrontendAction_75 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(75)

private:
    class MutatorASTConsumer_75 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_75(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::VarDecl*, std::string> arrayExpansions;
    };
};

//source file
#include "../include/Expand_Template_Arguments_In_Consteval_Context_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                VD->getLocation()))
            return;

        if (auto *TT = VD->getType()->getAs<clang::TemplateSpecializationType>()) {
            if (TT->getNumArgs() == 1) {
                if (auto *CE = dyn_cast<clang::ConstantExpr>(VD->getInit())) {
                    std::string expandedArray = "{";
                    for (int i = 0; i < 4; ++i) {
                        expandedArray += std::to_string(i + 1);
                        if (i < 3) expandedArray += ", ";
                    }
                    expandedArray += "}";
                    arrayExpansions[VD] = expandedArray;
                }
            }
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                CE->getBeginLoc()))
            return;

        if (CE->getNumArgs() == 0) {
            if (auto *DRE = dyn_cast<clang::DeclRefExpr>(CE->getCallee())) {
                if (auto *VD = dyn_cast<clang::VarDecl>(DRE->getDecl())) {
                    if (arrayExpansions.find(VD) != arrayExpansions.end()) {
                        std::string newCall = "transpose<" + arrayExpansions[VD] + ">()";
                        Rewrite.ReplaceText(CE->getSourceRange(), newCall);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher arrayMatcher = varDecl(hasType(templateSpecializationType()), hasInitializer(expr())).bind("arrayVar");
    StatementMatcher callMatcher = callExpr(callee(declRefExpr(to(functionDecl(hasName("transpose")))))).bind("templateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}