//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_union_with_forward_declaration_34
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
#include "modify_union_with_forward_declaration_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("UnionDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      if (!UD->isUnion())
        return;
      if (!UD->hasAttr<clang::TransparentUnionAttr>())
        return;

      //Get the source code text of target node
      auto unionText = stringutils::rangetoStr(*(Result.SourceManager),
                                               UD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string forwardDecl = "union ForwardDeclared; ";
      std::string newMember = "union ForwardDeclared member2; ";
      size_t insertPos = unionText.find("{") + 1;
      unionText.insert(insertPos, forwardDecl);
      size_t memberInsertPos = unionText.rfind("}");
      unionText.insert(memberInsertPos, newMember);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), unionText);
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(isUnion(), hasAttr(attr::TransparentUnion)).bind("UnionDecl");
    Callback callback(TheRewriter); 
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}