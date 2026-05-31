//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declare_Enum_With_Type_Traits_424
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
#include "../include/Forward_Declare_Enum_With_Type_Traits_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;

        std::string enumName = ED->getNameAsString();
        if (enumName.empty())
            return;

        std::vector<std::string> underlyingTypes = {"bool", "int", "char", "short", "long", "long long", "unsigned char", "unsigned short", "unsigned int", "unsigned long", "unsigned long long"};
        int typeIndex = getrandom::getRandomIndex(underlyingTypes.size());
        std::string underlyingType = underlyingTypes[typeIndex];

        std::string newEnumText = "enum " + enumName + " : " + underlyingType + ";";

        std::vector<std::string> traits = {
            "std::is_enum<" + enumName + ">::value",
            "!std::is_function<" + enumName + ">::value",
            "std::is_same<" + enumName + ", int>::value",
            "!std::is_same<" + enumName + ", bool>::value",
            "std::is_integral<" + enumName + ">::value",
            "!std::is_arithmetic<" + enumName + ">::value"
        };
        int traitIndex = getrandom::getRandomIndex(traits.size());
        std::string trait = traits[traitIndex];

        std::string staticAssertText = "static_assert(" + trait + ", \"\");";

        SourceLocation endLoc = ED->getEndLoc();
        Rewrite.ReplaceText(ED->getSourceRange(), newEnumText);
        Rewrite.InsertTextAfterToken(endLoc, "\n" + staticAssertText);
    }
}

void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(unless(hasDefinition())).bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}