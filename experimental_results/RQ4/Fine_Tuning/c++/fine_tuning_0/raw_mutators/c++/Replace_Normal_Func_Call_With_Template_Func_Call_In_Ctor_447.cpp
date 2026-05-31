//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Normal_Func_Call_With_Template_Func_Call_In_Ctor_447
 */ 
class MutatorFrontendAction_447 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(447)

private:
    class MutatorASTConsumer_447 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_447(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Normal_Func_Call_With_Template_Func_Call_In_Ctor_447.h"

// ========================================================================================================
#define MUT447_OUTPUT 1

void MutatorFrontendAction_447::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto methods = MT->methods();
      bool has_template_func = false;
      bool has_ctor = false;
      for (auto method : methods) {
        if (method->isTemplated())
          has_template_func = true;
        if (method->isImplicit() == false && method->isCopyOrMoveConstructor())
          has_ctor = true;
      }
      if (!has_ctor || !has_template_func)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << content;
      auto ctors = MT->ctors();
      for (auto ctor : ctors) {
        if (ctor->isImplicit() == false) {
          auto ctor_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                      ctor->getSourceRange());
          llvm::outs() << ctor_content;
          auto func_calls = ctor->getBody()->children();
          for (auto func_call : func_calls) {
            if (isa<CallExpr>(func_call)) {
              auto call_expr = dyn_cast<CallExpr>(func_call);
              auto callee = call_expr->getDirectCallee();
              if (callee->isTemplated() == false) {
                auto callee_name = callee->getNameAsString();
                llvm::outs() << callee_name;
                auto replace_pos = ctor_content.find(callee_name);
                if (replace_pos != string::npos) {
                  ctor_content.replace(replace_pos, callee_name.length(),
                                       "templateFunction<int>");
                }
              }
            }
          }
          content.replace(content.find(ctor_content), ctor_content.length(),
                          ctor_content);
        }
      }
      llvm::outs() << content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_447::MutatorASTConsumer_447::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}