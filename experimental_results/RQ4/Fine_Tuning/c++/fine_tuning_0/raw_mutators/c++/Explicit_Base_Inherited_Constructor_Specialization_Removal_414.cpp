//header file
#pragma once
#include "Mutator_base.h"

/**
 * Explicit_Base_Inherited_Constructor_Specialization_Removal_414
 */ 
class MutatorFrontendAction_414 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(414)

private:
    class MutatorASTConsumer_414 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_414(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Explicit_Base_Inherited_Constructor_Specialization_Removal_414.h"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->isCompleteDefinition())
        return;
      auto decls = MT->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          //Perform mutation on the source code text by applying string replacement
          llvm::outs() << "/*mut414*/"
                       << stringutils::rangetoStr(*(Result.SourceManager),
                                                  decl->getSourceRange())
                       << '\n';
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(decl->getSourceRange()),
                              "/*mut414*/");
        }
      }
    }
}
  
void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}