//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Address_Space_Attribute_To_Pointer_Member_392
 */ 
class MutatorFrontendAction_392 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(392)

private:
    class MutatorASTConsumer_392 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_392(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Apply_Address_Space_Attribute_To_Pointer_Member_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        // Filter nodes in header files
        if (!record || !Result.Context->getSourceManager().isWrittenInMainFile(record->getLocation()))
            return;

        // Check if it's a template
        if (!record->isTemplated())
            return;

        // Get the template parameters
        const clang::TemplateParameterList *params = record->getTemplateParameters();
        if (!params || params->size() == 0)
            return;

        const clang::TemplateParameter *firstParam = params->getParam(0);
        const auto *nonTypeParam = dyn_cast<clang::TemplateNonTypeParmDecl>(firstParam);
        if (!nonTypeParam)
            return;

        clang::QualType paramType = nonTypeParam->getType();
        if (!paramType->isIntegerType())
            return;

        std::string paramName = nonTypeParam->getNameAsString();
        if (paramName.empty())
            return;

        // Process each pointer field in the record
        for (const auto *field : record->fields()) {
            const auto *fieldDecl = dyn_cast<clang::FieldDecl>(field);
            if (!fieldDecl)
                continue;

            clang::QualType fieldType = fieldDecl->getType();
            if (!fieldType->isPointerType())
                continue;

            // Get the source code of the field declaration
            std::string fieldStr = stringutils::rangetoStr(*Result.SourceManager, fieldDecl->getSourceRange());

            // Find the position to insert the attribute
            size_t starPos = fieldStr.find_last_of('*');
            if (starPos == std::string::npos)
                continue;

            // Split into type and variable parts
            std::string typePart = fieldStr.substr(0, starPos + 1);
            std::string varPart = fieldStr.substr(starPos + 1);

            // Create the new string with the attribute
            std::string newStr = typePart + " [[clang::address_space(" + paramName + ")]] " + varPart;

            // Replace the field declaration with the new string
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(fieldDecl->getSourceRange()), newStr);
        }
    }
}

void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition(), hasField(fieldDecl(hasType(PointerType())).bind("field"))).bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}