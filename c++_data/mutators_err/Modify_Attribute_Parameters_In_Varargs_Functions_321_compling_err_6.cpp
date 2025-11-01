//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Attribute_Parameters_In_Varargs_Functions_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(321)

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Attribute_Parameters_In_Varargs_Functions_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("VarargsFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasAttr<clang::FormatArgAttr>()) {
        auto *Attr = FD->getAttr<clang::FormatArgAttr>();
        unsigned Index = Attr->getFormatIdx();
        unsigned NumParams = FD->getNumParams();

        // Modify the index to an invalid value
        unsigned NewIndex = NumParams + 1; // Exceeds the number of non-varargs parameters
        std::string NewAttrText = "__attribute__((__format_arg__(" + std::to_string(NewIndex) + ")))";

        SourceRange AttrRange = Attr->getRange();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(AttrRange), NewAttrText);
      }
    }
}
  
void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isVariadic(), hasAttr<clang::FormatArgAttr>()).bind("VarargsFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}