//header file
#pragma once
#include "Mutator_base.h"

/**
 * Faulty_Member_Access_On_Invalid_Types_277
 */ 
class MutatorFrontendAction_277 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(277)

private:
    class MutatorASTConsumer_277 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_277(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Faulty_Member_Access_On_Invalid_Types_277.h"

// ========================================================================================================
#define MUT277_OUTPUT 1

void MutatorFrontendAction_277::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto base = MT->getBase();
      auto base_type = base->getType();
      if (base_type->isBuiltinType() || base_type->isDependentType())
        return;
      auto base_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               base->getSourceRange());
      auto member_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << base_text << '\n';
      llvm::outs() << member_text << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (base_type->isStructureType() || base_type->isClassType()) {
        base_text = "int";
      }
      member_text = base_text + " " + member_text;
      member_text = "/*mut277*/" + member_text;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), 0, member_text);
    }
}
  
void MutatorFrontendAction_277::MutatorASTConsumer_277::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = memberExpr().bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}