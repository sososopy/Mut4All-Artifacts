//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Type_Trait_On_User_Defined_Type_332
 */ 
class MutatorFrontendAction_332 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(332)

private:
    class MutatorASTConsumer_332 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_332(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Type_Trait_On_User_Defined_Type_332.h"

// ========================================================================================================
#define MUT332_OUTPUT 1

void MutatorFrontendAction_332::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto aliasname = MT->getNameAsString();
      llvm::outs() << aliasname << '\n';
      if (aliasname.find("__add_rvalue_reference") == string::npos)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("__add_rvalue_reference") != string::npos) {
        content = stringutils::replaceFirst(content, "__add_rvalue_reference",
                                            "__add_pointer");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_332::MutatorASTConsumer_332::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}