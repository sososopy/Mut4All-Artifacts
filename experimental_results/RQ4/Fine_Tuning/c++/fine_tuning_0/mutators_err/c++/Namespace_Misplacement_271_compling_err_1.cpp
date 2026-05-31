//header file
#pragma once
#include "Mutator_base.h"

/**
 * Namespace_Misplacement_271
 */ 
class MutatorFrontendAction_271 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(271)

private:
    class MutatorASTConsumer_271 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_271(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Namespace_Misplacement_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto ins = "namespace mut_271 {using ::";
      auto fields = MT->fields();
      if (fields.empty())
        return;
      auto index = getrandom::getRandomIndex(fields.size() - 1);
      auto target = fields[index];
      ins += target->getNameAsString() + ";}\n";
      content.insert(content.rfind('}'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}