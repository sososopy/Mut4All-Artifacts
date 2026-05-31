//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedPointerToMemberSpecifier_155
 */ 
class MutatorFrontendAction_155 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(155)

private:
    class MutatorASTConsumer_155 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_155(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NestedPointerToMemberSpecifier_155.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *decl = Result.Nodes.getNodeAs<clang::Decl>("decl")) {
        if (!decl || !Result.Context->getSourceManager().isWrittenInMainFile(decl->getLocation()))
            return;

        if (!decl->getTypeSourceInfo())
            return;

        clang::TypeLoc typeLoc = decl->getTypeSourceInfo()->getTypeLoc();
        clang::NestedNameSpecifier *nested = typeLoc.getNestedNameSpecifier();
        if (!nested)
            return;

        QualType type = nested->getAsType();
        if (!type || !type->getAs<clang::RecordType>())
            return;

        SourceRange range = nested->getSourceRange();
        if (range.isInvalid())
            return;

        std::string original = stringutils::rangetoStr(*Result.SourceManager, range);
        if (original.empty())
            return;

        std::string modified;

        if (type->getAs<clang::TemplateSpecializationType>()) {
            size_t angleBracketPos = original.rfind('>');
            if (angleBracketPos != std::string::npos) {
                modified = original.substr(0, angleBracketPos + 1) + "* ";
            } else {
                modified = original + "* ";
            }
        } else {
            size_t lastColonPos = original.rfind("::");
            if (lastColonPos != std::string::npos) {
                modified = original.substr(0, lastColonPos + 2) + "* ";
            } else {
                modified = original + "* ";
            }
        }

        Rewrite.ReplaceText(range, modified);
    }
}

void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = decl().bind("decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}