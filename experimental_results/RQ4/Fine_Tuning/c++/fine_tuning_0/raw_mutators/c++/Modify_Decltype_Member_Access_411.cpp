//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Decltype_Member_Access_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(411)

private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Decltype_Member_Access_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto ret = DL->getAsFunction()->getReturnType();
      if (!ret->isDecltypeType())
        return;
      auto DT = ret->getAs<DecltypeType>();
      auto DT_expr = DT->getUnderlyingExpr();
      auto expr_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              DT_expr->getSourceRange());
      if (expr_str.find("::") == string::npos)
        return;
      auto pos = expr_str.find("::");
      expr_str.insert(pos, "::template NewNestedType");
      expr_str = "decltype(" + expr_str + ")";
      llvm::outs() << expr_str << '\n';
      Rewrite.ReplaceText(DT->getBeginLoc(), DT->getEndLoc(), expr_str);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
      DL_str.insert(DL_str.rfind('}'), "struct NewNestedType{};");
      DL_str = "/*mut411*/" + DL_str;
      Rewrite.ReplaceText(DL->getBeginLoc(), DL->getEndLoc(), DL_str);
    }
}
  
void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}