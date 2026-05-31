//source file
#include "../include/replace_enum_with_class_348.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        // Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;

        if (ED->isScoped()) // Only consider unscoped enums
            return;

        // Get the source code text of target node
        auto enumName = ED->getNameAsString();
        if (enumName.empty())
            return;

        // Record the enum for replacement
        enumsToReplace.push_back(ED);
    }

    for (const auto *ED : enumsToReplace) {
        std::string classDefinition = "class " + ED->getNameAsString() + " {\npublic:\n";
        for (const auto *EC : ED->enumerators()) {
            classDefinition += "static const int " + EC->getNameAsString() + " = ";
            classDefinition += std::to_string(EC->getInitVal().getSExtValue()) + ";\n";
        }
        classDefinition += "};\n";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), classDefinition);
    }
}

void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(unless(isScoped())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}