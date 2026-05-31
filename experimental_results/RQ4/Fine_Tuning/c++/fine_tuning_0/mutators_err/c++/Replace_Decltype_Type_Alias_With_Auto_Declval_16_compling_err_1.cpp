//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Type_Alias_With_Auto_Declval_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Decltype_Type_Alias_With_Auto_Declval_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isImplicit())
        return;
      if (DL->getTypeSourceInfo() == nullptr)
        return;
      auto target_type = DL->getTypeSourceInfo()->getType();
      if (target_type.getTypePtrOrNull() == nullptr)
        return;
      if (!target_type->isDecltypeType())
        return;
      auto type_string = stringutils::rangetoStr(*(Result.SourceManager),
                                                 DL->getSourceRange());
      auto type_string2 = stringutils::rangetoStr(*(Result.SourceManager),
                                                  DL->getTypeSourceInfo());
      llvm::outs() << type_string << '\n';
      llvm::outs() << type_string2 << '\n';
      type_string2 = type_string2.substr(8);
      llvm::outs() << type_string2 << '\n';
      type_string2 = "decltype(std::declval" + type_string2 + ")";
      llvm::outs() << type_string2 << '\n';
      type_string = "/*mut16*/" + type_string;
      type_string.replace(type_string.find("decltype"), type_string.size(),
                          type_string2);
      llvm::outs() << type_string << '\n';
      Rewrite.ReplaceText(DL->getSourceRange(), type_string);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}