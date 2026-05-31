//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Static_Assert_Contradiction_179
 */ 
class MutatorFrontendAction_179 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(179)

private:
    class MutatorASTConsumer_179 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_179(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Static_Assert_Contradiction_179.h"

// ========================================================================================================
#define MUT179_OUTPUT 1

void MutatorFrontendAction_179::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("template_class")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find("friend") == string::npos)
        return;
      auto friend_pos = content.find("friend");
      auto friend_end = content.find(';', friend_pos);
      auto friend_decl = content.substr(friend_pos, friend_end - friend_pos);
      llvm::outs() << friend_decl << '\n';
      if (friend_decl.find("static_assert") == string::npos)
        return;
      auto static_assert_pos = friend_decl.find("static_assert");
      auto static_assert_end = friend_decl.find(';', static_assert_pos);
      auto static_assert_decl =
          friend_decl.substr(static_assert_pos,
                             static_assert_end - static_assert_pos + 1);
      llvm::outs() << static_assert_decl << '\n';
      if (static_assert_decl.find('!') != string::npos)
        return;
      auto left_paren_pos = static_assert_decl.find('(');
      auto comma_pos = static_assert_decl.find(',');
      auto right_paren_pos = static_assert_decl.find(')');
      auto assert_condition = static_assert_decl.substr(
          left_paren_pos + 1, comma_pos - left_paren_pos - 1);
      auto assert_info = static_assert_decl.substr(
          comma_pos + 1, right_paren_pos - comma_pos - 1);
      llvm::outs() << assert_condition << '\n';
      llvm::outs() << assert_info << '\n';
      string new_static_assert_decl = "static_assert(!" + assert_condition +
                                      "," + assert_info + ");";
      llvm::outs() << new_static_assert_decl << '\n';
      //Perform mutation on the source code text by applying string replacement
      content.insert(friend_end + 1, new_static_assert_decl);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_179::MutatorASTConsumer_179::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isTemplateInstantiation()).bind("template_class");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}