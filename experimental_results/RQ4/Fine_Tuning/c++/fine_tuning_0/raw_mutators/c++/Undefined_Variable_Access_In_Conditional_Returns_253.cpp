//header file
#pragma once
#include "Mutator_base.h"

/**
 * Undefined_Variable_Access_In_Conditional_Returns_253
 */ 
class MutatorFrontendAction_253 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(253)

private:
    class MutatorASTConsumer_253 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_253(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Undefined_Variable_Access_In_Conditional_Returns_253.h"

// ========================================================================================================
#define MUT253_OUTPUT 1

void MutatorFrontendAction_253::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->hasBody())
        return;
      //Get the source code text of target node
      auto body = MT->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      std::vector<ReturnStmt *> return_stmts;
      std::vector<IfStmt *> if_stmts;
      std::vector<SwitchStmt *> switch_stmts;
      std::vector<CompoundStmt *> compound_stmts;
      std::vector<Stmt *> children;
      for (auto child : body->children()) {
        children.push_back(child);
        if (auto ret = dyn_cast<ReturnStmt>(child)) {
          return_stmts.push_back(ret);
        } else if (auto ifs = dyn_cast<IfStmt>(child)) {
          if_stmts.push_back(ifs);
        } else if (auto sws = dyn_cast<SwitchStmt>(child)) {
          switch_stmts.push_back(sws);
        } else if (auto cs = dyn_cast<CompoundStmt>(child)) {
          compound_stmts.push_back(cs);
        }
      }
      if (return_stmts.size() <= 1)
        return;
      if (if_stmts.empty() && switch_stmts.empty())
        return;
      if (!compound_stmts.empty())
        return;
      llvm::outs() << body_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      int dice = getrandom::getRandomIndex(return_stmts.size() - 1);
      llvm::outs() << dice << '\n';
      auto retstmt = return_stmts[dice];
      auto retstmt_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                 retstmt->getSourceRange());
      retstmt_str = "return mut_253";
      llvm::outs() << retstmt_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(retstmt->getSourceRange()), retstmt_str);
    }
}
  
void MutatorFrontendAction_253::MutatorASTConsumer_253::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}