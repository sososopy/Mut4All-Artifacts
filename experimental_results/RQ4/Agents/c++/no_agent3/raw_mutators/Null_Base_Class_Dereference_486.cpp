//header file
#pragma once
#include "Mutator_base.h"

/**
 * Null_Base_Class_Dereference_486
 */ 
class MutatorFrontendAction_486 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(486)

private:
    class MutatorASTConsumer_486 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_486(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Null_Base_Class_Dereference_486.h"

// ========================================================================================================
#define MUT486_OUTPUT 1

void MutatorFrontendAction_486::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
      //Filter nodes in header files
      if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DerivedClass->getLocation()))
        return;

      if (!DerivedClass->isCompleteDefinition() || DerivedClass->bases().empty())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string newBaseClass = "\nclass UndefinedBase; /*mut486*/\n";
      std::string mutatedCode = stringutils::rangetoStr(*(Result.SourceManager),
                                                        DerivedClass->getSourceRange());
      size_t pos = mutatedCode.find('{');
      if (pos != std::string::npos) {
        mutatedCode.insert(pos, ", public UndefinedBase");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DerivedClass->getSourceRange()), mutatedCode);
      Rewrite.InsertTextBefore(DerivedClass->getBeginLoc(), newBaseClass);
    }
}
  
void MutatorFrontendAction_486::MutatorASTConsumer_486::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}