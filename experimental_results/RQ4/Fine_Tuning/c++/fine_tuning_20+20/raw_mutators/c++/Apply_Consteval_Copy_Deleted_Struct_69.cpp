//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Consteval_Copy_Deleted_Struct_69
 */ 
class MutatorFrontendAction_69 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(69)

private:
    class MutatorASTConsumer_69 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_69(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *target_struct;
    };
};

//source file
#include "../include/Apply_Consteval_Copy_Deleted_Struct_69.h"

// ========================================================================================================
#define MUT69_OUTPUT 1

void MutatorFrontendAction_69::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->isCompleteDefinition())
        return;
      if (ST->hasUserDeclaredConstructor() == false)
        return;
      bool has_consteval_ctor = false;
      auto ctors = ST->ctors();
      for (auto ctor : ctors) {
        if (ctor->isConsteval()) {
          has_consteval_ctor = true;
          break;
        }
      }
      if (has_consteval_ctor == false)
        return;
      if (ST->hasUserDeclaredCopyConstructor() == false)
        return;
      auto copy_ctor = ST->getCopyConstructor();
      if (copy_ctor->isDeleted() == false)
        return;
      llvm::outs() << "mut69 find target struct\n";
      target_struct = ST;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNameAsString() != "main")
        return;
      if (target_struct == nullptr)
        return;
      auto body = FD->getBody();
      if (!body)
        return;
      auto stmts = body->children();
      for (auto stmt : stmts) {
        if (auto *EX = dyn_cast<Expr>(stmt)) {
          auto type = EX->getType();
          if (type->isStructureType() == false)
            continue;
          auto decl = type->getAsCXXRecordDecl();
          if (decl == target_struct) {
            auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   EX->getSourceRange());
            llvm::outs() << "mut69 find target expr\n";
            if (content.find("{}") != string::npos) {
              content.insert(content.find("{}") + 1, content);
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(EX->getSourceRange()),
                  content);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_69::MutatorASTConsumer_69::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Struct");
    auto main_matcher = functionDecl().bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}