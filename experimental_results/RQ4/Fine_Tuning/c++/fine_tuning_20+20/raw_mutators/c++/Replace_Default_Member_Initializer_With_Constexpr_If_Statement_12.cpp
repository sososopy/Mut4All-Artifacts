//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Member_Initializer_With_Constexpr_If_Statement_12
 */ 
class MutatorFrontendAction_12 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(12)

private:
    class MutatorASTConsumer_12 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_12(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/replace_default_member_initializer_with_constexpr_if_statement_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    cur_classes.push_back(DL);
  } else if (auto *FL = Result.Nodes.getNodeAs<clang::FieldDecl>("Fields")) {
    if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FL->getLocation()))
      return;
    if (FL->hasInClassInitializer() == false)
      return;
    auto init = FL->getInClassInitializer();
    if (init == nullptr)
      return;
    auto init_type = init->getStmtClass();
    if (init_type != Stmt::StmtClass::LambdaExprClass)
      return;
    auto DL = FL->getParent();
    bool flag = false;
    for (size_t i = 0; i < cur_classes.size(); ++i) {
      if (DL == cur_classes[i]) {
        flag = true;
        break;
      }
    }
    if (flag == false)
      return;
    auto lambda = dyn_cast<LambdaExpr>(init);
    auto body = lambda->getBody();
    auto body_type = body->getStmtClass();
    if (body_type != Stmt::StmtClass::CompoundStmtClass)
      return;
    auto comp = dyn_cast<CompoundStmt>(body);
    auto stmts = comp->body();
    if (stmts.size() != 1)
      return;
    auto stmt_type = stmts[0]->getStmtClass();
    if (stmt_type != Stmt::StmtClass::ReturnStmtClass)
      return;
    auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                        init->getSourceRange());
    auto replace_text =
        "[=]{if constexpr(false){int x=1/0;}else{return 0;}}";
    llvm::outs() << text << " " << replace_text << '\n';
    Rewrite.ReplaceText(init->getSourceRange(), replace_text);
  }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
  auto field_matcher = fieldDecl().bind("Fields");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.addMatcher(field_matcher, &callback);
  matchFinder.matchAST(Context);
}