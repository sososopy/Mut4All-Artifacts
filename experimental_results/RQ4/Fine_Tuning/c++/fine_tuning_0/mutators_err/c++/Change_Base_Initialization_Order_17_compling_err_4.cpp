//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Base_Initialization_Order_17
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
#include "../include/Change_Base_Initialization_Order_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (MT->getNumBases() <= 1)
        return;
      //Get the source code text of target node
      auto bases = MT->bases();
      std::vector<string> base_names;
      for (auto base : bases) {
        auto base_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 base.getSourceRange());
        base_names.push_back(base_name);
      }
      auto ctors = MT->ctors();
      for (auto ctor : ctors) {
        if (ctor->isImplicit())
          continue;
        auto inits = ctor->inits();
        std::vector<clang::CXXCtorInitializer *> inits_vec;
        for (auto init : inits) {
          inits_vec.push_back(init);
        }
        if (inits_vec.size() <= 1)
          continue;
        std::vector<string> init_names;
        for (auto init : inits_vec) {
          if (init->isMemberInitializer())
            continue;
          auto init_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                   init->getSourceRange());
          init_names.push_back(init_name);
        }
        if (init_names.size() <= 1)
          continue;
        //Perform mutation on the source code text by applying string replacement
        std::swap(init_names[0], init_names[1]);
        string new_ctor = "";
        for (size_t i = 0; i < init_names.size(); ++i) {
          new_ctor += init_names[i];
          if (i != init_names.size() - 1)
            new_ctor += ",";
        }
        llvm::outs() << new_ctor << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(inits_vec[0]->getSourceRange().getBegin(),
                            inits_vec[init_names.size() - 1]->getSourceRange().getEnd().getLocWithOffset(1),
                            new_ctor);
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}