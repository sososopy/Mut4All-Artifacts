//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_With_Integer_Seq_296
 */ 
class MutatorFrontendAction_296 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(296)

private:
    class MutatorASTConsumer_296 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_296(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> validTemplates;
        std::vector<std::string> existingTypes;
        std::vector<int> existingIntegers;
    };

};

//source file
#include "../include/Replace_Template_With_Integer_Seq_296.h"

// ========================================================================================================
#define MUT296_OUTPUT 1

void MutatorFrontendAction_296::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check for template declarations with one type parameter
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
        if (!TD || !TD->getTemplatedDecl() || !TD->getTemplatedDecl()->isDefinition())
            return;
        auto *TParamList = TD->getTemplateParameters();
        if (TParamList && TParamList->size() == 1) {
            std::string templateName = TD->getNameAsString();
            validTemplates.push_back(templateName);
        }
    }

    // Collect existing types from variable declarations
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        std::string typeName = VD->getType().getAsString();
        existingTypes.push_back(typeName);
    }

    // Collect existing types from field declarations
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        std::string typeName = FD->getType().getAsString();
        existingTypes.push_back(typeName);
    }

    // Collect existing integer literals
    else if (auto *IL = Result.Nodes.getNodeAs<clang::IntegerLiteral>("integerLiteral")) {
        if (!IL)
            return;
        llvm::APSInt value = IL->getValue();
        if (value.isSigned())
            existingIntegers.push_back(value.getSExtValue());
        else
            existingIntegers.push_back(value.getZExtValue());
    }

    // Process variable declarations with non-template types
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("mutVarDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (validTemplates.empty() || existingTypes.empty() || existingIntegers.empty())
            return;

        // Randomly select template, type, and integer
        int templateIdx = getrandom::getRandomIndex(validTemplates.size());
        int typeIdx = getrandom::getRandomIndex(existingTypes.size());
        int intIdx = getrandom::getRandomIndex(existingIntegers.size());

        std::string newType = "__make_integer_seq<" + validTemplates[templateIdx] + ", " + existingTypes[typeIdx] + ", " + std::to_string(existingIntegers[intIdx]) + ">";

        // Replace the original type with newType
        SourceLocation start = VD->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        SourceLocation end = VD->getTypeSourceInfo()->getTypeLoc().getEndLoc();
        Rewrite.ReplaceText(SourceRange(start, end), newType);
    }

    // Process field declarations with non-template types
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("mutFieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (validTemplates.empty() || existingTypes.empty() || existingIntegers.empty())
            return;

        int templateIdx = getrandom::getRandomIndex(validTemplates.size());
        int typeIdx = getrandom::getRandomIndex(existingTypes.size());
        int intIdx = getrandom::getRandomIndex(existingIntegers.size());

        std::string newType = "__make_integer_seq<" + validTemplates[templateIdx] + ", " + existingTypes[typeIdx] + ", " + std::to_string(existingIntegers[intIdx]) + ">";

        SourceLocation start = FD->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        SourceLocation end = FD->getTypeSourceInfo()->getTypeLoc().getEndLoc();
        Rewrite.ReplaceText(SourceRange(start, end), newType);
    }
}

void MutatorFrontendAction_296::MutatorASTConsumer_296::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    // Match templates with one type parameter
    DeclarationMatcher templateMatcher = templateDecl(
        isDefinition(),
        hasTemplateParameters(templateParamList(hasNumTemplateParams(1)))
    ).bind("templateDecl");

    // Match variable declarations with non-template types
    DeclarationMatcher varMatcher = varDecl(
        unless(isImplicit()),
        hasType(unless(isTemplateInstantiation()))
    ).bind("mutVarDecl");

    // Match field declarations with non-template types
    DeclarationMatcher fieldMatcher = fieldDecl(
        unless(isImplicit()),
        hasType(unless(isTemplateInstantiation()))
    ).bind("mutFieldDecl");

    // Match integer literals
    DeclarationMatcher intMatcher = integerLiteral().bind("integerLiteral");

    // Match variable declarations to collect types
    DeclarationMatcher collectVarMatcher = varDecl().bind("varDecl");

    // Match field declarations to collect types
    DeclarationMatcher collectFieldMatcher = fieldDecl().bind("fieldDecl");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(intMatcher, &callback);
    matchFinder.addMatcher(collectVarMatcher, &callback);
    matchFinder.addMatcher(collectFieldMatcher, &callback);
    matchFinder.matchAST(Context);
}