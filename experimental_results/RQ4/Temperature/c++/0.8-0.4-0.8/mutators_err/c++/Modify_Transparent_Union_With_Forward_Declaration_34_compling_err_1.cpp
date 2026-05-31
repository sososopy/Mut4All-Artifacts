//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_transparent_union_with_forward_declaration_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_transparent_union_with_forward_declaration_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UT = Result.Nodes.getNodeAs<clang::TypedefDecl>("TransparentUnion")) {
      //Filter nodes in header files
      if (!UT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UT->getLocation()))
        return;

      if (auto *UD = dyn_cast<clang::RecordDecl>(UT->getUnderlyingType()->getAsTagDecl())) {
        if (UD->hasAttr<clang::TransparentUnionAttr>()) {
          //Get the source code text of target node
          auto unionSource = stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          std::string forwardDecl = "struct ForwardType;";
          std::string mutatedUnion = "typedef union { ForwardType *ptr; " + unionSource.substr(unionSource.find("{") + 1);
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextBefore(UT->getBeginLoc(), forwardDecl + "\n");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(UT->getSourceRange()), mutatedUnion);
        }
      }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasType(recordDecl(hasAttr<TransparentUnionAttr>()))).bind("TransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}