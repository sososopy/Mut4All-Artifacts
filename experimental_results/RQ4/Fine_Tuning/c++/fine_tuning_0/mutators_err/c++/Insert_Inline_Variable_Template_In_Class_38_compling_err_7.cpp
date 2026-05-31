//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Inline_Variable_Template_In_Class_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)
    
    private:
      class MutatorASTConsumer_38 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_inline_variable_template_in_class_38"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    //Perform mutation on the source code text by applying string replacement
    auto inlinevar = "\n\t/*mut38*/template<typename T> inline auto mut_38=T(0);";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), inlinevar);
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    cur_classes.push_back(DL);
  } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>(
                 "Return0Stmt")) { // If there is a return 0 statement in the function, replace the return value with the inserted inline variable template
    if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   RS->getBeginLoc()))
      return;

    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    //Get the source code text of target node
    auto target = cur_classes[index];
    //Perform mutation on the source code text by applying string replacement
    auto ins = "/*mut38*/return " + target->getNameAsString() + "::mut_38<int>";
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), ins);
  }
}

void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto matcher = cxxRecordDecl().bind("Classes");
    auto return0Stmt_macher =
        returnStmt(hasReturnValue(integerLiteral(equals(0))))
            .bind("Return0Stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(return0Stmt_macher, &callback);
    matchFinder.matchAST(Context);
}