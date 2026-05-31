//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutation_On_Function_End_268
 */ 
class MutatorFrontendAction_268 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(268)

private:
    class MutatorASTConsumer_268 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_268(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutation_On_Function_End_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isThisDeclarationADefinition())
        return;
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      auto body = FD->getBody();
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto body_begin_pos = body_content.find('{');
      auto body_end_pos = body_content.rfind('}');
      assert(body_begin_pos != string::npos);
      assert(body_end_pos != string::npos);
      auto body_sub_content = body_content.substr(body_begin_pos + 1,
                                                  body_end_pos - body_begin_pos - 1);
      llvm::outs() << body_sub_content << '\n';
      if (body_sub_content == "")
        return;
      auto statements = llvm::split(body_sub_content, ';');
      if (statements.empty())
        return;
      int choice = getrandom::getRandomIndex(statements.size() - 1);
      auto target_statement = statements[choice];
      llvm::outs() << target_statement << '\n';
      auto target_statement_content =
          llvm::split(target_statement, ' ');
      if (target_statement_content.empty())
        return;
      auto target_variable = target_statement_content.back();
      llvm::outs() << target_variable << '\n';
      auto ins = "/*mut268*/" + target_variable + " + ";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(body->getEndLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}