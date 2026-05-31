//header file
#pragma once
#include "Mutator_base.h"

/**
 * Bitwise_Enum_Using_67
 */ 
class MutatorFrontendAction_67 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(67)
private:
    class MutatorASTConsumer_67 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_67(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        struct TypeAliasEntry {
            const clang::TypeAliasDecl *typeAlias;
            const clang::EnumDecl *enumDecl;
        };
        std::vector<TypeAliasEntry> storedTypeAliases;
    };
};

//source file
#include "../include/Bitwise_Enum_Using_67.h"

// ========================================================================================================
#define MUT67_OUTPUT 1

void MutatorFrontendAction_67::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!record || !Result.Context->getSourceManager().isWrittenInMainFile(record->getLocation()))
            return;

        auto *enumDecl = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl");
        auto *typeAlias = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("typeAlias");
        if (!enumDecl || !typeAlias)
            return;

        storedTypeAliases.push_back(TypeAliasEntry{typeAlias, enumDecl});
    } else if (auto *function = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
        if (!function || !Result.Context->getSourceManager().isWrittenInMainFile(function->getLocation()))
            return;

        auto *usingEnum = Result.Nodes.getNodeAs<clang::UsingEnumDecl>("usingEnum");
        if (!usingEnum)
            return;

        auto *underlyingEnum = dyn_cast<clang::EnumDecl>(usingEnum->getUnderlyingDecl());
        if (!underlyingEnum)
            return;

        QualType usingEnumType = underlyingEnum->getIntegerType();
        for (const auto &entry : storedTypeAliases) {
            QualType typeAliasType = entry.typeAlias->getUnderlyingType();
            if (usingEnumType.getCanonicalType() != typeAliasType.getCanonicalType())
                continue;

            auto *enumRef = Result.Nodes.getNodeAs<clang::DeclRefExpr>("enumRef");
            if (!enumRef)
                continue;

            auto *enumDecl = entry.enumDecl;
            SmallVector<clang::EnumConstantDecl*, 4> constants;
            for (auto *enumerator : enumDecl->enumerators())
                constants.push_back(enumerator);

            if (constants.size() < 2)
                continue;

            int index1 = getrandom::getRandomIndex(constants.size());
            int index2 = getrandom::getRandomIndex(constants.size() - 1);
            if (index2 >= index1)
                index2++;

            clang::EnumConstantDecl *const1 = constants[index1];
            clang::EnumConstantDecl *const2 = constants[index2];

            std::string newName = "(" + const1->getNameAsString() + " | " + const2->getNameAsString() + ")";
            Rewrite.ReplaceText(enumRef->getSourceRange(), newName);
        }
    }
}
  
void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = cxxRecordDecl(
        has(enumDecl().bind("enumDecl")),
        has(typeAliasDecl(hasType(refersToType(typeOf(enumDecl(equalsBoundNode("enumDecl"))))))).bind("typeAlias")
    ).bind("record");
    DeclarationMatcher matcher2 = functionDecl(
        has(usingEnumDecl().bind("usingEnum")),
        hasDescendant(declRefExpr(to(enumConstantDecl())).bind("enumRef"))
    ).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}