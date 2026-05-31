//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reorder_Base_Classes_In_Struct_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)

private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/reorder_base_classes_in_struct_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (ST->isStruct() == false)
        return;
      if (ST->isCompleteDefinition() == false)
        return;
      //Get the source code text of target node
      auto bases = ST->bases();
      if (bases.size() < 2)
        return;
      std::vector<string> base_names;
      for (auto base : bases) {
        auto base_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 base.getSourceRange());
        base_names.push_back(base_name);
      }
      std::reverse(base_names.begin(), base_names.end());
      string new_bases = "";
      for (auto base_name : base_names) {
        if (new_bases == "")
          new_bases += base_name;
        else
          new_bases += (", " + base_name);
      }
      llvm::outs() << new_bases << '\n';
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(bases.front().getBeginLoc(),
                          bases.back().getEndLoc(), new_bases);
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}