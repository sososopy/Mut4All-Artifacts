//header file
#pragma once
#include "Mutator_base.h"

/**
 * Implicit_Member_Access_In_Default_Initializer_Within_Template_6
 */ 
class MutatorFrontendAction_6 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(6)

private:
    class MutatorASTConsumer_6 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_6(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Implicit_Member_Access_In_Default_Initializer_Within_Template_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Ordinary))
        return;
      if (MT->isImplicit())
        return;
      if (MT->isAnonymousStructOrUnion())
        return;
      if (MT->isBitField())
        return;
      if (MT->isStaticDataMember())
        return;
      if (MT->hasInClassInitializer() == false)
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      auto inclassinit = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getInClassInitializer()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << "name: " << name << " init: " << inclassinit << '\n';
      auto replacement = name;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getInClassInitializer()->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasParent(cxxRecordDecl(hasParent(classTemplateDecl())))).bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}