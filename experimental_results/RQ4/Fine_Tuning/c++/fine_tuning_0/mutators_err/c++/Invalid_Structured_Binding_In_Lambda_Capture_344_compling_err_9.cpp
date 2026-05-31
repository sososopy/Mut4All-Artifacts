//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Structured_Binding_In_Lambda_Capture_344
 */ 
class MutatorFrontendAction_344 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(344)

private:
    class MutatorASTConsumer_344 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_344(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Invalid_Structured_Binding_In_Lambda_Capture_344.h"

// ========================================================================================================
#define MUT344_OUTPUT 1

void MutatorFrontendAction_344::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(DL);
  } else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
    if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   LE->getBeginLoc()))
      return;
    //Get the source code text of target node
    auto captures = LE->capture_init_begin();
    std::vector<string> capture_names;
    for (auto capture = captures; capture != LE->capture_init_end(); capture++) {
      if ((*capture)->getStmtClass() == Stmt::StmtClass::DeclRefExprClass) {
        auto name = cast<DeclRefExpr>(*capture)->getNameInfo().getAsString();
        capture_names.push_back(name);
      }
    }
    if (capture_names.empty())
      return;
    llvm::outs() << capture_names.size() << '\n';
    size_t index = getrandom::getRandomIndex(capture_names.size() - 1);
    llvm::outs() << index << '\n';
    auto target = capture_names[index];
    //Perform mutation on the source code text by applying string replacement
    auto target_decl = "/*mut344*/auto &[" + target + ",_]=_";
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(LE->getBeginLoc(), 0, target_decl);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Decl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isStructuredBinding())
      return;
    //Get the source code text of target node
    auto type = DL->getType();
    if (type->isClassType()) {
      auto record = type->getAsCXXRecordDecl();
      if (record->isCompleteDefinition() == false)
        return;
      auto fields = record->fields();
      std::vector<string> field_names;
      for (auto field : fields) {
        field_names.push_back(field->getNameAsString());
      }
      if (field_names.empty())
        return;
      size_t index = getrandom::getRandomIndex(field_names.size() - 1);
      auto target = field_names[index];
      //Perform mutation on the source code text by applying string replacement
      auto target_decl = "/*mut344*/auto &[" + target + ",_]=_";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getBeginLoc(), 0, target_decl);
    }
  }
}

void MutatorFrontendAction_344::MutatorASTConsumer_344::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto lambda_matcher = lambdaExpr().bind("Lambda");
    auto struct_bind_matcher = varDecl().bind("Decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(struct_bind_matcher, &callback);
    matchFinder.matchAST(Context);
}