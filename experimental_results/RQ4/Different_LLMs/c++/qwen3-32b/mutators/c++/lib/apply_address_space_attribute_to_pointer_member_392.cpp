//source file
#include "../include/Apply_Address_Space_Attribute_To_Pointer_Member_392.h"
#include "clang/ASTMatchers/ASTMatchers.h"

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

        const clang::TemplateParameterList *params = record->getTemplateParameterList(0);
        if (!params || params->size() == 0)
            return;

        const clang::NamedDecl *firstParam = params->getParam(0);
        if (!firstParam)
            return;

        const auto *nonTypeParam = dyn_cast<clang::NonTypeTemplateParmDecl>(firstParam);
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
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition(), has(fieldDecl(hasType(pointerType())).bind("field"))).bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}