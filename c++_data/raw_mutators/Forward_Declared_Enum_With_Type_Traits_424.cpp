//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declared_Enum_With_Type_Traits_424
 */ 
class MutatorFrontendAction_424 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(424)

private:
    class MutatorASTConsumer_424 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_424(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/forward_declared_enum_with_type_traits_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("ForwardDeclaredEnum")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
            return;

        if (!ED->isCompleteDefinition()) {
            std::string enumName = ED->getNameAsString();
            std::string replacement = "enum " + enumName + " : int;";

            Rewrite.ReplaceText(ED->getSourceRange(), replacement);

            std::string staticAssert = "static_assert(!std::is_enum<" + enumName + ">::value, \"\");";
            Rewrite.InsertTextAfterToken(ED->getEndLoc(), "\n/*mut424*/" + staticAssert);
        }
    }
}
  
void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(unless(isDefinition())).bind("ForwardDeclaredEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}