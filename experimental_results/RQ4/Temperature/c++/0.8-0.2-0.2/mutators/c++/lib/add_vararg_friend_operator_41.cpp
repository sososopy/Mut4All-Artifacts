//source file
#include "../include/add_vararg_friend_operator_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SR = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
        if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(SR->getLocation()))
            return;

        if (SR->isStruct() && SR->hasDefinition()) {
            auto sourceRange = SR->getSourceRange();
            std::string sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                                          *Result.SourceManager, 
                                                          Result.Context->getLangOpts()).str();
            std::string newFriendFunction = "\nfriend void operator,(MyStruct, ...); // New variadic friend function\n";
            size_t insertPosition = sourceText.rfind('}');
            if (insertPosition != std::string::npos) {
                sourceText.insert(insertPosition, newFriendFunction);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
            }
        }
    }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), hasDefinition(), has(friendDecl())).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}