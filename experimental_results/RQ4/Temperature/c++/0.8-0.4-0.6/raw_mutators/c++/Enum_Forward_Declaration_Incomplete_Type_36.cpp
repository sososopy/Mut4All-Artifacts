//header file
#pragma once
#include "Mutator_base.h"

/**
 * enum_forward_declaration_incomplete_type_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/enum_forward_declaration_incomplete_type_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *EnumDecl = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!EnumDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                EnumDecl->getLocation()))
            return;

        if (!EnumDecl->isScoped() || EnumDecl->isCompleteDefinition())
            return;

        std::string enumName = "NewEnumClass";
        std::string newEnumDecl = "enum struct " + enumName + ";";

        SourceLocation insertLoc = EnumDecl->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n" + newEnumDecl);

        std::string checkerTemplate = "\ntemplate <class T>\nstruct Checker : decltype(someFunction<T>(0)) {};\n";
        std::string staticAssert = "static_assert(Checker<" + enumName + ">::value);";

        Rewrite.InsertTextAfterToken(insertLoc, checkerTemplate + staticAssert);
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(isScoped(), unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}