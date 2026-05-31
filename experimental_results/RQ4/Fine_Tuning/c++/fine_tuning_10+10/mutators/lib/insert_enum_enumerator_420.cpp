//source file
#include "../include/Insert_Enum_Enumerator_420.h"

// ========================================================================================================
#define MUT420_OUTPUT 1

void MutatorFrontendAction_420::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        //Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;
        
        //Get the source code text of target node
        auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        std::string newEnumerator = "NEW_ENUMERATOR"; // Name should follow existing naming conventions
        std::string insertion = ", " + newEnumerator;

        // Find the position to insert the new enumerator
        size_t insertPos = enumSource.rfind('}');
        if (insertPos != std::string::npos) {
            enumSource.insert(insertPos, insertion);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumSource);
    }
}

void MutatorFrontendAction_420::MutatorASTConsumer_420::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}