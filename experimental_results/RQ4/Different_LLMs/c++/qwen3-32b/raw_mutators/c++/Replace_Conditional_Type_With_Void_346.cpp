//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Conditional_Type_With_Void_346
 */ 
class MutatorFrontendAction_346 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(346)
private:
    class MutatorASTConsumer_346 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_346(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Conditional_Type_With_Void_346.h"

// ========================================================================================================
#define MUT346_OUTPUT 1

void MutatorFrontendAction_346::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("alias")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation())) return;
      auto typeLoc = MT->getTypeSourceInfo()->getTypeLoc();
      if (auto tspec = typeLoc.getAs<clang::TemplateSpecializationTypeLoc>()) {
        auto args = tspec.getTemplateArgs();
        if (args.size() < 3) return;
        auto thirdArg = args[2];
        if (thirdArg.getKind() == clang::TemplateArgument::Type) {
          auto type = thirdArg.getAsType();
          if (auto voidType = type->getAs<clang::BuiltinType>()) {
            if (voidType->getKind() == clang::BuiltinType::Void) return;
          }
        }
        auto argsLoc = tspec.getTemplateArgsLoc();
        if (argsLoc.size() < 3) return;
        SourceLocation thirdStart = argsLoc[2].getBeginLoc();
        SourceLocation thirdEnd = argsLoc[2].getEndLoc();
        if (thirdStart.isInvalid() || thirdEnd.isInvalid()) return;
        CharSourceRange thirdRange = CharSourceRange::getCharRange(thirdStart, thirdEnd);
        Rewrite.ReplaceText(thirdRange, "void");
      }
    }
}
  
void MutatorFrontendAction_346::MutatorASTConsumer_346::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasType(templateSpecializationType(hasName("conditional_t")))).bind("alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}