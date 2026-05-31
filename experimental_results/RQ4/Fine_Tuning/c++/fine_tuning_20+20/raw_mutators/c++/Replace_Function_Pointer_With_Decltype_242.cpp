//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Pointer_With_Decltype_242
 */ 
class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(242)

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
        const clang::VarDecl *target_decl;
    };
};

//source file
#include "../include/Replace_Function_Pointer_With_Decltype_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FP = Result.Nodes.getNodeAs<clang::VarDecl>("FunctionPointer")) {
      if (!FP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FP->getLocation()))
        return;
      if (FP->getType()->isFunctionPointerType()) {
        llvm::outs() << "Find a function pointer\n";
        auto DL = FP->getType()->getPointeeType()->getAs<FunctionProtoType>();
        if (DL->getTemplateSpecializationArgsAsWritten() != nullptr) {
          target_decl = FP;
        }
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::DeclStmt>("DeclStmt")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isSingleDecl() == false)
        return;
      auto decl = DL->getSingleDecl();
      if (decl == target_decl) {
        llvm::outs() << "Find the target declstmt\n";
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        llvm::outs() << content << '\n';
        if (content.find('=') != string::npos) {
          content = "/*mut242*/decltype(auto) " +
                    content.substr(content.find_first_of(' '));
          llvm::outs() << content << '\n';
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(DL->getSourceRange()), content);
        }
      }
    } else if (auto *AS = Result.Nodes.getNodeAs<clang::BinaryOperator>(
                   "AssignStmt")) {
      if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AS->getBeginLoc()))
        return;
      if (AS->getOpcode() != BinaryOperatorKind::BO_Assign)
        return;
      auto LHS = AS->getLHS();
      if (LHS->getType()->isFunctionPointerType()) {
        auto DL = LHS->getType()->getPointeeType()->getAs<FunctionProtoType>();
        if (DL->getTemplateSpecializationArgsAsWritten() != nullptr) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 AS->getSourceRange());
          llvm::outs() << content << '\n';
          auto lhs = stringutils::rangetoStr(*(Result.SourceManager),
                                             LHS->getSourceRange());
          content = "/*mut242*/decltype(auto) " + lhs + " = " +
                    content.substr(content.find('=') + 1);
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(AS->getSourceRange()), content);
        }
      }
    }
}

void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("FunctionPointer");
    auto declstmt_matcher = declStmt().bind("DeclStmt");
    auto assign_matcher = binaryOperator().bind("AssignStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(declstmt_matcher, &callback);
    matchFinder.addMatcher(assign_matcher, &callback);
    matchFinder.matchAST(Context);
}