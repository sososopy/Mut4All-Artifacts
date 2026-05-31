//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Destructor_Declaration_172
 */ 
class MutatorFrontendAction_172 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(172)

private:
    class MutatorASTConsumer_172 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_172(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Destructor_Declaration_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto TD = CL->getDestructor();
      if (TD == nullptr)
        return;
      auto def =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      string dtor_definition = "~" + def + "()=default;\n";
      dtor_definition = "/*mut172*/" + dtor_definition;
      //Replace the original AST node with the mutated one
      if (TD->isImplicit()) {
        llvm::outs() << dtor_definition << '\n';
        Rewrite.ReplaceText(CL->getEndLoc(), 0, dtor_definition);
      } else {
        auto dtor_def = stringutils::rangetoStr(*(Result.SourceManager),
                                                TD->getSourceRange());
        auto choice = getrandom::getRandomIndex(1);
        if (choice) {
          Rewrite.ReplaceText(TD->getSourceRange(),
                              "/*mut172*/" + dtor_def + "=default");
        } else {
          Rewrite.ReplaceText(TD->getSourceRange(), "/*mut172*/" + dtor_def);
        }
      }
    }
}
  
void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}