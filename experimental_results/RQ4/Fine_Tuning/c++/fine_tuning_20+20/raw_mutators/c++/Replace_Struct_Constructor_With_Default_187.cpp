//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Constructor_With_Default_187
 */ 
class MutatorFrontendAction_187 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(187)

private:
    class MutatorASTConsumer_187 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_187(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Struct_Constructor_With_Default_187.h"

// ========================================================================================================
#define MUT187_OUTPUT 1

void MutatorFrontendAction_187::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isInStdNamespace() && !MT->isImplicit() && MT->isStructOrUnion() &&
          MT->getParent()->isStruct()) {
        if (MT->getNumParams() == 0)
          return;
        //Get the source code text of target node
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        llvm::outs() << content << '\n';
        auto pos = content.find('(');
        assert(pos != string::npos && "Mut187: Can't find '(' in the constructor");
        content.replace(pos, content.size() - pos, "()=default;");
        content = "/*mut187*/" + content;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MT->getSourceRange(), content);
      }
    }
}
  
void MutatorFrontendAction_187::MutatorASTConsumer_187::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}