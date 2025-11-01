//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Constexpr_Static_Member_Value_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)
    
    private:
      class MutatorASTConsumer_13 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<int> cur_classes_type;  //Store the type of the inserted static value; if it is private or protected, it will no longer be considered for use
      };
    };

//source file
#include "../include/insert_constexpr_static_member_value_13"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    int dice = getrandom::getRandomIndex(2);
    std::string ins_type = "";
    if (dice == 0)
      ins_type = "private";
    else if (dice == 1)
      ins_type = "protected";
    else if (dice == 2)
      ins_type = "public";
    else
      assert(false && "Mut13: Dice should have 3 possible values!");
    //Perform mutation on the source code text by applying string replacement
    auto constexrdecl =
        "\n\t/*mut13*/" + ins_type + ":\n\tstatic constexpr int mut_13=0;\n";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), constexrdecl);
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    cur_classes.push_back(DL);
    cur_classes_type.push_back(dice);
  } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>(
                 "Return0Stmt")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
    if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   RS->getBeginLoc()))
      return;

    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    if (cur_classes_type[index] != 2) // must be public
      return;
    //Get the source code text of target node
    auto target = cur_classes[index];
    //Perform mutation on the source code text by applying string replacement
    auto ins = "/*mut13*/return " + target->getNameAsString() + "::mut_13";
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), ins);
  }
}

void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
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